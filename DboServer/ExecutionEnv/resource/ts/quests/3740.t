CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3740;
	title = 374002;

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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 374014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 374001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2532149;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 374009;
				ctype = 1;
				idx = 5592111;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 374008;
				gtype = 1;
				area = 374001;
				goal = 374004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 374005;
				stype = 2;
				taid = 1;
				title = 374002;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 374008;
			gtype = 1;
			oklnk = 2;
			area = 374001;
			goal = 374004;
			sort = 374005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 374002;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "252;0;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 374007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSClickNPC
			{
				npcidx = "5592111;";
			}
			CDboTSCheckClrQst
			{
				and = "3739;";
				flink = 0;
				flinknextqid = "3741;";
				not = 0;
			}
		}
	}
}

