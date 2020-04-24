CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2524;
	title = 252402;

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
				conv = 252409;
				ctype = 1;
				idx = 8512154;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
				idx0 = 1378;
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
				cont = 252408;
				m3widx = 14;
				m1fy = "0.000000";
				m2widx = 14;
				grade = 132203;
				m1pat = 2;
				m1widx = 14;
				m3fx = "-416.000000";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				m4fx = "-363.000000";
				m4ttip = 252415;
				area = 252401;
				goal = 252404;
				m0fz = "-54.000000";
				m0widx = 14;
				m1ttip = 252415;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "150.000000";
				m3ttip = 252415;
				scitem = -1;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-25.000000";
				m2fx = "-514.000000";
				m2ttip = 252415;
				m4widx = 14;
				m0fx = "-865.000000";
				m0ttip = 252415;
				m3pat = 2;
				m4pat = 2;
				title = 252402;
				m1fx = "-746.000000";
				m2fz = "28.000000";
				sort = 252405;
				m4fz = "8.000000";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 252408;
			gtype = 2;
			oklnk = 2;
			area = 252401;
			goal = 252404;
			sort = 252405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 252402;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 252414;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 252401;
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
				npcidx = "8512154;";
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
			stdiag = 252407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 53;
			}
			CDboTSCheckClrQst
			{
				and = "2523;";
				flink = 1;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "8512154;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 252407;
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
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1378;
				taid = 1;
				type = 1;
			}
		}
	}
}

