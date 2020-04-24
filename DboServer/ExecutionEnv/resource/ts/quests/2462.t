CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2462;
	title = 246202;

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
			cont = 246208;
			gtype = 2;
			oklnk = 2;
			area = 246201;
			goal = 246204;
			sort = 246205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 246202;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 246214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 246201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "8031502;";
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
			stdiag = 246207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 53;
			}
			CDboTSClickNPC
			{
				npcidx = "8031502;";
			}
			CDboTSCheckClrQst
			{
				and = "2461;";
				flink = 1;
				flinknextqid = "2463;";
				not = 0;
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
				conv = 246209;
				ctype = 1;
				idx = 8031502;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 7114202;
				cnt0 = 11;
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
				cont = 246208;
				m1fx = "-759.000000";
				m1fy = "0.000000";
				sort = 246205;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "1197.000000";
				grade = 132203;
				m0fx = "-816.000000";
				m0ttip = 246215;
				m1pat = 2;
				m1widx = 14;
				rwd = 100;
				taid = 1;
				title = 246202;
				gtype = 2;
				area = 246201;
				goal = 246204;
				m0fz = "978.000000";
				m0widx = 14;
				m1ttip = 246215;
				scitem = -1;
				stype = 2;
			}
		}
	}
}

