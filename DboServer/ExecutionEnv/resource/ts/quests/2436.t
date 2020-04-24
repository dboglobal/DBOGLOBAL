CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2436;
	title = 243602;

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
				conv = 243609;
				ctype = 1;
				idx = 5531501;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 243608;
				gtype = 1;
				area = 243601;
				goal = 243604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 243605;
				stype = 1;
				taid = 1;
				title = 243602;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 243614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 243601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3291202;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 243608;
			gtype = 1;
			oklnk = 2;
			area = 243601;
			goal = 243604;
			sort = 243605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 243602;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 243607;
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
				and = "2435;2431;";
				flink = 1;
				flinknextqid = "2437;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "5531501;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

