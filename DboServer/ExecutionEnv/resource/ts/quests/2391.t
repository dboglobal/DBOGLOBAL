CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2391;
	title = 239102;

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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 239109;
				ctype = 1;
				idx = 7092108;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 1;
				eitype = 1;
				idx0 = 1346;
				cnt0 = 1;
				cnt2 = 1;
				ectype = -1;
				etype = 0;
				idx1 = 1347;
				taid = 3;
				esctype = 0;
				idx2 = 1348;
			}
			CDboTSActRegQInfo
			{
				cont = 239108;
				m3widx = 14;
				m1fy = "0.000000";
				m2widx = 14;
				grade = 132203;
				m1pat = 2;
				m1widx = 14;
				m3fx = "399.000000";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				m4fx = "692.000000";
				m4ttip = 239117;
				area = 239101;
				goal = 239104;
				m0fz = "2.000000";
				m0widx = 14;
				m1ttip = 239116;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "110.000000";
				m3ttip = 239115;
				scitem = -1;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "58.000000";
				m2fx = "490.000000";
				m2ttip = 239117;
				m4widx = 14;
				m0fx = "485.000000";
				m0ttip = 239115;
				m3pat = 2;
				m4pat = 2;
				title = 239102;
				m1fx = "671.000000";
				m2fz = "242.000000";
				sort = 239105;
				m4fz = "266.000000";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 239114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 239101;
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
				npcidx = "7092108;";
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
			stdiag = 239107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = "7092108;";
			}
			CDboTSCheckClrQst
			{
				and = "2390;";
				flink = 1;
				flinknextqid = "2392;";
				not = 0;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 52;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 239108;
			gtype = 2;
			oklnk = 2;
			area = 239101;
			goal = 239104;
			sort = 239105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 239102;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 239107;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
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
				icnt2 = 255;
				iprob2 = "1.000000";
				icnt1 = 255;
				iidx0 = 1346;
				iidx2 = 1348;
				iprob1 = "1.000000";
				taid = 1;
				type = 1;
				icnt0 = 255;
				iidx1 = 1347;
				iprob0 = "1.000000";
			}
		}
	}
}

