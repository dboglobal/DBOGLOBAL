CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 18;
	rq = 0;
	tid = 396;
	title = 39602;

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
			prelnk = "100;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 39607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 10;
			}
			CDboTSCheckClrQst
			{
				and = "393;";
				flink = 1;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "6413108;";
			}
			CDboTSCheckPCRace
			{
				raceflg = 2;
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
				conv = 39609;
				ctype = 1;
				idx = 6413108;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 39608;
				gtype = 2;
				area = 39601;
				goal = 39604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 39605;
				stype = 64;
				taid = 1;
				title = 39602;
			}
			CDboTSActItem
			{
				iidx0 = 1041003;
				stype0 = 1;
				taid = 3;
				type = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 39608;
			gtype = 2;
			oklnk = 2;
			area = 39601;
			goal = 39604;
			sort = 39605;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 39602;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 39614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 39601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6413108;";
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSHoipoiMix
			{
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
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 1041003;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 107;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

