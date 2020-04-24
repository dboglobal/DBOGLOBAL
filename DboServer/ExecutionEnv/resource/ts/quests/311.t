CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 311;
	title = 31102;

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
				conv = 31109;
				ctype = 1;
				idx = 4752201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 31108;
				gtype = 1;
				area = 31101;
				goal = 31104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 31105;
				stype = 2;
				taid = 1;
				title = 31102;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 31114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 31101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511602;";
			}
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 31108;
			gtype = 1;
			oklnk = 2;
			area = 31101;
			goal = 31104;
			sort = 31105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 31102;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 31107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 16;
			}
			CDboTSClickNPC
			{
				npcidx = "4752201;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "312;";
				not = 0;
			}
		}
	}
}

