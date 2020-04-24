CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2432;
	title = 243202;

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
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 243208;
			gtype = 2;
			oklnk = 2;
			area = 243201;
			goal = 243204;
			sort = 243205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 243202;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 243214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 243201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3362102;";
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
			stdiag = 243207;
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
				and = "2431;";
				flink = 1;
				flinknextqid = "2433;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3362102;";
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
				conv = 243209;
				ctype = 1;
				idx = 3362102;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 6;
				eitype = 0;
				idx0 = 4612107;
				cnt0 = 8;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 4611208;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 243208;
				m3widx = 14;
				m1fy = "0.000000";
				m2widx = 14;
				grade = 132203;
				m1pat = 2;
				m1widx = 14;
				m3fx = "-519.000000";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				m4fx = "-420.000000";
				m4ttip = 243216;
				area = 243201;
				goal = 243204;
				m0fz = "361.000000";
				m0widx = 14;
				m1ttip = 243215;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "1122.000000";
				m3ttip = 243216;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "674.000000";
				m2fx = "-552.000000";
				m2ttip = 243215;
				m4widx = 14;
				m0fx = "-258.000000";
				m0ttip = 243215;
				m3pat = 2;
				m4pat = 2;
				title = 243202;
				m1fx = "-453.000000";
				m2fz = "957.000000";
				sort = 243205;
				m4fz = "1092.000000";
			}
		}
	}
}

