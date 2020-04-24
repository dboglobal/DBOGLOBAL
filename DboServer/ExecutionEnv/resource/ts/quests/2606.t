CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2606;
	title = 260602;

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
				conv = 260609;
				ctype = 1;
				idx = 5101102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 260608;
				gtype = 1;
				area = 260601;
				goal = 260604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 260605;
				stype = 2;
				taid = 1;
				title = 260602;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 260608;
			gtype = 1;
			oklnk = 2;
			area = 260601;
			goal = 260604;
			sort = 260605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 260602;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 260614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 260601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1657102;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 260607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickNPC
			{
				npcidx = "5101102;";
			}
			CDboTSCheckClrQst
			{
				and = "2605;";
				flink = 1;
				flinknextqid = "2607;";
				not = 0;
			}
		}
	}
}

