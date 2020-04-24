CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2572;
	title = 257202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 1;
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
				itype = 6;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 257208;
			gtype = 2;
			oklnk = 2;
			area = 257201;
			goal = 257204;
			sort = 257205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 257202;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 257214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 257201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7073110;";
			}
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
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 257209;
				ctype = 1;
				idx = 7073110;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 257208;
				gtype = 2;
				area = 257201;
				goal = 257204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 257205;
				stype = 2;
				taid = 1;
				title = 257202;
			}
			CDboTSActSToCEvt
			{
				widx1 = 1;
				ectype = -1;
				iidx0 = -1;
				otype1 = 1;
				dt0 = 257221;
				esctype = 0;
				iidx1 = -1;
				it1 = 257222;
				otype0 = 1;
				dt1 = 257223;
				etype = 0;
				it0 = 257220;
				taid = 3;
				tidx1 = 4751312;
				widx0 = 1;
				apptype = 0;
				eitype = 6;
				tidx0 = 4371316;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 257207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2573;2574;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "7073110;";
			}
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

