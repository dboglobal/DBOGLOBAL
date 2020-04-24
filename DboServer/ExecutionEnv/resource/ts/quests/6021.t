CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 6021;
	title = 602102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 253;
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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 602109;
				ctype = 1;
				idx = 3181301;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 602108;
				area = 602101;
				goal = 602104;
				m0fz = "-2214.439941";
				m0widx = 1;
				scitem = -1;
				sort = 602105;
				stype = 8;
				m0fy = "-76.286003";
				m0pat = 2;
				grade = -1;
				m0fx = "6500.109863";
				m0ttip = 602115;
				rwd = 100;
				taid = 1;
				title = 602102;
				gtype = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 3;
				idx0 = 626;
				cnt0 = 10;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 254;
			cid = 1;
			cont = 602108;
			gtype = 1;
			oklnk = 2;
			area = 602101;
			goal = 602104;
			sort = 602105;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 602102;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;0;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "6;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 602107;
			nolnk = 254;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = "3181301;";
			}
			CDboTSCheckClrQst
			{
				and = "6020;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 602114;
			nextlnk = 6;
			rwdexp = 0;
			rwdtbl = 602101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 10;
				iidx = 626;
			}
			CDboTSClickNPC
			{
				npcidx = "3181301;";
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 3;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 626;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 2;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 602107;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

