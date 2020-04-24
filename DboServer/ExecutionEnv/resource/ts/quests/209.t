CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 209;
	title = 20902;

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
				conv = 20909;
				ctype = 1;
				idx = 8511103;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 20908;
				gtype = 1;
				area = 20901;
				goal = 20904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 20905;
				stype = 64;
				taid = 1;
				title = 20902;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 20907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = "8511103;";
			}
			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 3;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "210;";
				not = 0;
				or = "208;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 20908;
			gtype = 1;
			oklnk = 2;
			area = 20901;
			goal = 20904;
			sort = 20905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 20902;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 20914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 20901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5041148;";
			}
		}
	}
}

