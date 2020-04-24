CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 722;
	title = 72202;

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
				conv = 72209;
				ctype = 1;
				idx = 4152112;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 72208;
				gtype = 1;
				area = 72201;
				goal = 72204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 72205;
				stype = 2;
				taid = 1;
				title = 72202;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 72214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 72201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1311202;";
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
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 72208;
			gtype = 1;
			oklnk = 2;
			area = 72201;
			goal = 72204;
			sort = 72205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 72202;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 72207;
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
				minlvl = 24;
			}
			CDboTSCheckClrQst
			{
				and = "721;";
				flink = 1;
				flinknextqid = "760;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4152112;";
			}
		}
	}
}

