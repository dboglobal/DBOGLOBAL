CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 7;
	title = 702;

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
				conv = 709;
				ctype = 1;
				idx = 7131101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 708;
				gtype = 1;
				area = 701;
				goal = 704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 705;
				stype = 64;
				taid = 1;
				title = 702;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 3;
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSClickNPC
			{
				npcidx = "7131101;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "8;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 708;
			gtype = 1;
			oklnk = 2;
			area = 701;
			goal = 704;
			sort = 705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 702;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "8512101;";
			}
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
	}
}

