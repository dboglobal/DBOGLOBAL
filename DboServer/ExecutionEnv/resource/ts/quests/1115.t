CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 6;
	rq = 0;
	tid = 1115;
	title = 111502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 100;
			prelnk = "4;";

			CDboTSActWorldPlayScript
			{
				sid = 1115;
				start = 0;
				taid = 1;
				uof = 1;
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
				eitype = 0;
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
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 111509;
				ctype = 1;
				idx = 4041110;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 6012201;
				cnt0 = 1;
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
				cont = 111508;
				m1fx = "-209.000000";
				m1fy = "0.000000";
				sort = 111505;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "1790.000000";
				grade = 132203;
				m0fx = "-232.000000";
				m0ttip = 111515;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 111502;
				gtype = 2;
				area = 111501;
				goal = 111504;
				m0fz = "1510.000000";
				m0widx = 1;
				m1ttip = 111515;
				scitem = -1;
				stype = 2;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 111507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 47;
			}
			CDboTSClickNPC
			{
				npcidx = "4041110;";
			}
			CDboTSCheckClrQst
			{
				and = "1114;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 252;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActWorldPlayScript
			{
				sid = 1115;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 5;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 111508;
			gtype = 2;
			oklnk = 2;
			area = 111501;
			goal = 111504;
			sort = 111505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 111502;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "3;2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 111514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 111501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4041110;";
			}
		}
	}
}

