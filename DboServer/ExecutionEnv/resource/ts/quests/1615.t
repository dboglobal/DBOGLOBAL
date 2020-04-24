CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1615;
	title = 161502;

	CNtlTSGroup
	{
		gid = 0;

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
				eitype = 1;
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
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 161508;
			gtype = 3;
			oklnk = 2;
			area = 161501;
			goal = 161504;
			sort = 161505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 161502;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 161514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 161501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 1;
			}
			CDboTSClickNPC
			{
				npcidx = "1651109;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 161507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 49;
			}
			CDboTSClickNPC
			{
				npcidx = "1651109;";
			}
			CDboTSCheckClrQst
			{
				and = "1614;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 161509;
				ctype = 1;
				idx = 1651109;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
				idx0 = 987;
				cnt0 = 3;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 161508;
				m1fx = "244.000000";
				m1fy = "0.000000";
				sort = 161505;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-253.000000";
				grade = 132203;
				m0fx = "-53.000000";
				m0ttip = 161515;
				m1pat = 2;
				m1widx = 10;
				rwd = 100;
				taid = 1;
				title = 161502;
				gtype = 3;
				area = 161501;
				goal = 161504;
				m0fz = "-477.000000";
				m0widx = 10;
				m1ttip = 161515;
				scitem = -1;
				stype = 2;
			}
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
				iidx0 = 987;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 161507;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

