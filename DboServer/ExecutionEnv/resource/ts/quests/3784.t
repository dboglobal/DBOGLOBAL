CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 25;
	rq = 0;
	tid = 3784;
	title = 378402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 9;
			elnk = 255;
			nextlnk = 10;
			prelnk = "8;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 378424;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 9;
			prelnk = "11;11;7;6;5;";

			CDboTSActPortal
			{
				dx = "0.994000";
				px = "-4388.062988";
				widx = 15;
				dy = "0.000000";
				py = "-115.227997";
				taid = 2;
				type = 0;
				dz = "0.106000";
				pz = "-5240.642090";
			}
		}
		CDboTSContNarration
		{
			cancellnk = 8;
			cid = 11;
			dt = 2;
			lilnk = 255;
			oklnk = 8;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "7;";
			dg = 378423;
			gt = 0;
			oi = 1641201;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 378407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 69;
			}
			CDboTSClickNPC
			{
				npcidx = "3333201;";
			}
			CDboTSCheckClrQst
			{
				and = "3783;";
				flink = 0;
				flinknextqid = "3787;";
				not = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 10;
			prelnk = "9;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActNPCConv
			{
				conv = 378420;
				ctype = 1;
				idx = 1641201;
				taid = 1;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 8;
			cid = 7;
			dt = 2;
			lilnk = 255;
			oklnk = 11;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "6;";
			dg = 378422;
			gt = 0;
			oi = 1641201;
		}
		CDboTSContNarration
		{
			cancellnk = 8;
			cid = 6;
			dt = 2;
			lilnk = 255;
			oklnk = 7;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "5;";
			dg = 378421;
			gt = 0;
			oi = 1641201;
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSClickNPC
			{
				npcidx = "1641201;";
			}
		}
		CDboTSContNarration
		{
			cancellnk = 8;
			cid = 5;
			dt = 2;
			lilnk = 255;
			oklnk = 6;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "4;";
			dg = 378420;
			gt = 0;
			oi = 1641201;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
				idx0 = 1634;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActNPCConv
			{
				conv = 378409;
				ctype = 1;
				idx = 5553103;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 378408;
				gtype = 3;
				area = 378401;
				goal = 378404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 378405;
				stype = 2;
				taid = 1;
				title = 378402;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "252;0;1;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
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
			cont = 378408;
			gtype = 3;
			oklnk = 2;
			area = 378401;
			goal = 378404;
			sort = 378405;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 378402;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 378414;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 378401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "10;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5631101;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 378407;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1634;
				taid = 1;
				type = 1;
			}
		}
	}
}

