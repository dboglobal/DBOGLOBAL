CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 422;
	title = 42202;

	CNtlTSGroup
	{
		gid = 0;

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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 42207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 5;
			}
			CDboTSClickNPC
			{
				npcidx = "4261101;";
			}
			CDboTSCheckClrQst
			{
				and = "420;";
				flink = 1;
				flinknextqid = "423;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 1812103;
				cnt0 = 6;
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
				cont = 42208;
				m1fx = "5979.520020";
				m1fy = "0.000000";
				sort = 42205;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-3423.500000";
				grade = 132203;
				m0fx = "6240.209961";
				m0ttip = 42215;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 42202;
				gtype = 2;
				area = 42201;
				goal = 42204;
				m0fz = "-3606.580078";
				m0widx = 1;
				m1ttip = 42215;
				scitem = -1;
				stype = 16;
			}
			CDboTSActNPCConv
			{
				conv = 42209;
				ctype = 1;
				idx = 4261101;
				taid = 2;
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
				itype = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 42208;
			gtype = 2;
			oklnk = 2;
			area = 42201;
			goal = 42204;
			sort = 42205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 42202;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 42214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 42201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4261101;";
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
	}
}

