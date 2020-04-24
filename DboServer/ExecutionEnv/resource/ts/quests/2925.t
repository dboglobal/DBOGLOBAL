CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2925;
	title = 292502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 292509;
				ctype = 1;
				idx = 5441401;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 292508;
				m3widx = 1;
				m1fy = "0.000000";
				sort = 292505;
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "2616.889893";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				area = 292501;
				goal = 292504;
				m0fz = "-3805.290039";
				m0widx = 1;
				m1ttip = 292515;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-4021.879883";
				m3ttip = 292515;
				scitem = -1;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-3776.489990";
				m2fx = "2652.520020";
				m2ttip = 292515;
				m0fx = "2358.709961";
				m0ttip = 292515;
				m3pat = 2;
				title = 292502;
				m1fx = "2477.969971";
				m2fz = "-3984.979980";
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 3;
				idx0 = 1523;
				cnt0 = 12;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 3;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 292507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 55;
			}
			CDboTSClickNPC
			{
				npcidx = "5441401;";
			}
			CDboTSCheckClrQst
			{
				and = "2921;";
				flink = 1;
				flinknextqid = "2926;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;252;0;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 292508;
			gtype = 2;
			oklnk = 2;
			area = 292501;
			goal = 292504;
			sort = 292505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 292502;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 292514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 292501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 3;
			}
			CDboTSClickNPC
			{
				npcidx = "5441401;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 292507;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1523;
				taid = 1;
				type = 1;
			}
		}
	}
}

