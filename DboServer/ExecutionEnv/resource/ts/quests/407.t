CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 407;
	title = 40702;

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
				conv = 40709;
				ctype = 1;
				idx = 8411104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 40708;
				gtype = 1;
				area = 40701;
				goal = 40704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 40705;
				stype = 64;
				taid = 1;
				title = 40702;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 40714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 40701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7131104;";
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 40707;
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
				minlvl = 3;
			}
			CDboTSClickNPC
			{
				npcidx = "8411104;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "408;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 40708;
			gtype = 1;
			oklnk = 2;
			area = 40701;
			goal = 40704;
			sort = 40705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 40702;
		}
	}
}

